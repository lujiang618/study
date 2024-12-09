import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";

// 创建场景
const scene = new THREE.Scene();

// 创建相机
const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000
);
camera.position.z = 50;

// 创建渲染器
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// 添加环境光
const ambientLight = new THREE.AmbientLight(0x888888);
scene.add(ambientLight);

// 添加点光源
const pointLight = new THREE.PointLight(0xffffff, 1, 100);
pointLight.position.set(10, 10, 10);
scene.add(pointLight);

let controls, stats;

// 创建粒子几何体和材质
const createParticles = (count, size, color) => {
    const particles = new THREE.BufferGeometry();
    const positions = new Float32Array(count * 3);
    const velocities = new Float32Array(count * 3);

    for (let i = 0; i < count; i++) {
        positions[i * 3] = (Math.random() - 0.5) * 200;
        positions[i * 3 + 1] = (Math.random() - 0.5) * 200;
        positions[i * 3 + 2] = (Math.random() - 0.5) * 200;

        velocities[i * 3] = 0;
        velocities[i * 3 + 1] = -Math.random() * 0.8;
        velocities[i * 3 + 2] = 0;
    }

    particles.setAttribute("position", new THREE.BufferAttribute(positions, 3));
    particles.setAttribute(
        "velocity",
        new THREE.BufferAttribute(velocities, 3)
    );

    const material = new THREE.ShaderMaterial({
        uniforms: {
            followMatrix: { value: new THREE.Matrix4() }, // Uniform 用于传递跟随矩阵
        },
        vertexShader: `
            uniform mat4 followMatrix;
            void main() {
                // 将顶点位置变换到跟随矩阵的空间
                // vec4 transformedPosition = followMatrix * vec4(position, 1.0);

                // 传递到 gl_Position
                gl_Position = projectionMatrix * modelViewMatrix * followMatrix * vec4(position, 1.0);
                gl_PointSize = 3.0;
            }
        `,
        fragmentShader: `
            void main() {
                gl_FragColor = vec4(1.0, 1.0, 0.0, 0.6); // 简单白色粒子
            }
        `,
    });

    return new THREE.Points(particles, material);
};

const geometry = new THREE.BoxGeometry(1, 1, 1);
const material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
const cube = new THREE.Mesh(geometry, material);
scene.add(cube);

// 创建雨、雪和雾的粒子系统
// const rainParticles = createParticles(1000, 0.3, 0xff0000);
// scene.add(rainParticles);

const snowParticles = createParticles(10000, 0.2, 0x0000ff);
scene.add(snowParticles);

// const fogParticles = createParticles(500, 0.2, 0x00ff00);
// scene.add(fogParticles);

const followMatrix = new THREE.Matrix4(); // 用于存储跟随矩阵

function updateFollowMatrix(camera, offset) {
    // 重置矩阵为单位矩阵
    // followMatrix.identity();

    // 从相机的世界矩阵复制位置和旋转信息
    // followMatrix.copy(camera.matrixWorld);

    // 将偏移向量转换到世界空间并添加到矩阵
    const offsetWorld = offset.clone().applyMatrix4(camera.matrixWorld);
    followMatrix.copy(camera.matrixWorld);
}

// 更新粒子系统
const updateParticles = (particleSystem, matrix) => {
    const positions = particleSystem.geometry.attributes.position.array;
    const velocities = particleSystem.geometry.attributes.velocity.array;

    for (let i = 0; i < positions.length / 3; i++) {
        positions[i * 3 + 1] += velocities[i * 3 + 1];

        if (positions[i * 3 + 1] < -100) {
            positions[i * 3 + 1] = 100;
        }
    }

    particleSystem.geometry.attributes.position.needsUpdate = true;

    // 更新跟随矩阵
    const offset = new THREE.Vector3(0, 0, 0);
    updateFollowMatrix(camera, offset);

    particleSystem.matrix.copy(followMatrix);
    particleSystem.matrixAutoUpdate = false;

    console.log(particleSystem.matrix.elements,particleSystem.matrixWorld.elements);
};

controls = new OrbitControls(camera, renderer.domElement);
controls.target.set(0, 10, 0);
controls.minDistance = 1;
controls.maxDistance = 15000;
controls.maxPolarAngle = Math.PI / 1.7;
controls.autoRotate = false;
controls.autoRotateSpeed = -1;
controls.update();

stats = new Stats();
document.body.appendChild(stats.dom);

const animate = () => {
    controls.update();
    requestAnimationFrame(animate);

    // updateParticles(rainParticles, camera.matrixWorld);
    updateParticles(snowParticles, camera.matrixWorld);
    // updateParticles(fogParticles, camera.matrixWorld);

    renderer.render(scene, camera);

    stats.update();
};

animate();
